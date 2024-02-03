#!/bin/bash

docker build -t mongo_server mongo_db
docker run -e MONGO_INITDB_ROOT_USERNAME=admin \
      -e MONGO_INITDB_ROOT_PASSWORD=mot_de_sel_passe \
      mongo_server

sleep 10

mongoimport --host=172.17.0.2:27017 \
            --db new_york --collection restaurants \
            --username admin --password mot_de_sel_passe \
            --authenticationDatabase admin \
            --file ../restaurants.json