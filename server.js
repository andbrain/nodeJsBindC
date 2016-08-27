"use strict";

var currentPath = process.cwd();
var restify = require('restify');
var bunyan  = require('bunyan');

var options = {
	name: "Akiry Search Engine API"
};

var server = restify.createServer(options);

restify.CORS.ALLOW_HEADERS.push('authorization');
server.use(restify.acceptParser(server.acceptable));

server.pre(restify.CORS());
server.use(restify.fullResponse());

server.use(restify.authorizationParser());
server.use(restify.queryParser({ mapParams: false }));

//max body size 10kb
server.use(restify.bodyParser({
    maxBodySize: 10 * 1024,
    mapParams: false,
    uploadDir: currentPath + '/tmp'
}));

// Add headers
server.use(function (req, res, next) {

    // Website you wish to allow to connect
    // res.setHeader('Access-Control-Allow-Origin', 'http://localhost:8080');
    res.setHeader('Access-Control-Allow-Origin', '*');

    // Request methods you wish to allow
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE');

    // Request headers you wish to allow
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,Content-Type, Accept');

    // Set to true if you need the website to include cookies in the requests sent
    // to the API (e.g. in case you use sessions)
    // res.setHeader('Access-Control-Allow-Credentials', true);

    // Pass to next layer of middleware
    next();
});

server.on('after', restify.auditLogger({
    log: bunyan.createLogger({
        name: 'audit',
        streams: [
            {
                level: 'warn',
                path: './log/warn.log'
            },
            {
                level: 'info',
                path: './log/info.log'
            },
            {
                level: 'error',
                path: './log/error.log'
            },
            {
                level: 'trace',
                stream: process.stdout
            },
            {
                level: 'fatal',
                path: './log/fatal.log'
            }
        ]
    })
}));

module.exports = server;
require('./routes');