// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

const path = require('path')
const fs = require('fs-extra')
const webpack = require('webpack')
const GenerateDepfilePlugin = require('./webpack-plugin-depfile')
const pathMap = require('./path-map')

const tsConfigPath = path.join(process.env.ROOT_GEN_DIR, 'tsconfig-webpack.json')

module.exports = async function (env, argv) {
  const isDevMode = argv.mode === 'development'
  // Webpack config object
  const resolve = {
    extensions: ['.js', '.tsx', '.ts', '.json'],
    alias: pathMap,
    modules: [ 'node_modules' ]
  }

  if (argv.extra_modules) {
    resolve.modules = [
      ...(
        Array.isArray(argv.extra_modules)
        ? argv.extra_modules
        : [argv.extra_modules]
      ),
      ...resolve.modules
    ]
  }

  if (argv.webpack_alias) {
    resolve.aliasFields = Array.isArray(argv.webpack_alias) ? argv.webpack_alias : [ argv.webpack_alias ]
  }
  return {
    devtool: isDevMode ? '#inline-source-map' : false,
    output: {
      path: process.env.TARGET_GEN_DIR,
      filename: '[name].bundle.js',
      chunkFilename: '[id].chunk.js'
    },
    resolve,
    optimization: {
      // Define NO_CONCATENATE for analyzing module size.
      concatenateModules: !process.env.NO_CONCATENATE
    },
    plugins: process.env.DEPFILE_SOURCE_NAME ? [
      new GenerateDepfilePlugin({
        depfilePath: process.env.DEPFILE_PATH,
        depfileSourceName: process.env.DEPFILE_SOURCE_NAME
      })
    ] : [],
    module: {
      rules: [
        {
          // CSS imported from node_modules or in a x.global.css file
          // is just regular css converted to JS and injected to style elements
          test: /\.s?css$/,
          include: [/\.global\./, /node_modules/],
          use: [
            { loader: "style-loader" },
            { loader: "css-loader" },
          ],
        },
        {
          // CSS imported in the source tree can use sass and css modules
          // syntax.
          test: /\.s?css$/,
          exclude: [/\.global\./, /node_modules/],
          use: [
            // Injects the result into the DOM as a style block
            { loader: "style-loader" },
            // Converts the resulting CSS to Javascript to be bundled
            // (modules:true to rename CSS classes in output to cryptic identifiers,
            // except if wrapped in a :global(...) pseudo class).
            {
              loader: "css-loader",
              options: {
                importLoaders: 3,
                sourceMap: false,
                modules: {
                  localIdentName: isDevMode
                    ? "[path][name]__[local]--[hash:base64:5]"
                    : "[hash:base64]",
                },
              },
            },
             // First, convert SASS to CSS
            { loader: "sass-loader" },
          ],
        },
        {
          test: /\.tsx?$/,
          loader: 'ts-loader',
          exclude: /node_modules\/(?!brave-ui)/,
          options: {
            getCustomTransformers: path.join(__dirname, './webpack-ts-transformers.js'),
            allowTsInNodeModules: true,
            // Use generated tsconfig so that we can point at gen/ output in the
            // correct build configuration output directory.
            configFile: tsConfigPath
          }
        },
        // Loads font files for Font Awesome
        {
          test: /\.woff(2)?(\?v=[0-9]\.[0-9]\.[0-9])?$/,
          loader: 'url-loader?limit=13000&minetype=application/font-woff'
        },
        {
          test: /\.(ttf|eot|ico|svg|png|jpg|jpeg|gif)(\?v=[0-9]\.[0-9]\.[0-9])?$/,
          loader: 'file-loader'
        }]
    },
    node: {
      fs: 'empty'
    }
  }
}
