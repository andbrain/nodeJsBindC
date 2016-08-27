# Binder C++ for NodeJS
Binder c++ for use as service in nodejs

# Configuration
* How to install?
	1. Install npm package node-gyp
	> npm install -g node-gyp

* How to use?
	- Generate appropriate project build files for the current platform, use _configure_:
	> node-gyp configure

	- Look for *binding.gyp* and *makefile* files in the build directory, to create this files, see [here](https://github.com/nodejs/node-gyp), after that:
	> node-gyp build

# Usage
* Building correctly, you require de addon from build directory
> var addon = require('./build/Release/addon');
> addon.method();
