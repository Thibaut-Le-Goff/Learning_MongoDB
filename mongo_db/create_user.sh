#!/bin/bash

#mongo --eval 'db.createUser({user: "admin", pwd: passwordPrompt(), roles: ["root"], mechanisms: ["SCRAM-SHA-256"]})' --host localhost:27017 --username admin --password admin

echo "db.createUser({ user: 'admin', pwd: $1, roles: [{ role: 'root', db: 'admin' }], mechanisms: ['SCRAM-SHA-256'], passwordDigestor: 'server' });" >> /docker-entrypoint-initdb.d/users.js
