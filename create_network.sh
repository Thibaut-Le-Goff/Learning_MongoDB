#!/bin/bash

docker network create --subnet 172.50.0.0/16 POC_MongoDB


# for the server
docker pull mongo

docker build -t mongo_server mongo_db

docker run --net POC_MongoDB \
            --ip 172.50.0.2 \
            --cap-add=NET_ADMIN \
            --name sever \
            --hostname POC_MongoDB \
            -e MONGO_INITDB_ROOT_USERNAME=admin \
            -e MONGO_INITDB_ROOT_PASSWORD=mot_de_sel_passe \
            mongo_server &

sleep 10

mongoimport --host=172.50.0.2:27017 \
            --db new_york \
            --collection restaurants \
            --username admin \
            --password mot_de_sel_passe \
            --authenticationDatabase admin \
            --file ../restaurants.json


# for the client
docker pull mongodb/mongo-cxx-driver:latest

docker build -t mongo_client mongo_client

docker run --net POC_MongoDB \
           --ip 172.50.0.3 \
           --cap-add=NET_ADMIN \
           -it --name client \
           --hostname POC_MongoDB mongo_client
