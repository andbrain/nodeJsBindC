"use strict";

var restify = require('restify');
var currentPath = process.cwd();
var server = require(currentPath + '/server');
var resources = require(currentPath + '/resources');
var prefix = 'api/';
var VERSION = '1.0.0';

var resource = '/search';
var PATH = prefix + resource;

server.get({path: PATH, version: VERSION}, resources.search.list);