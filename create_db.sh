#!/bin/bash

docker build -t mongo_server mongo_db
docker run mongo_server

# l'importation des donnée ne marche pas via le script
#mongoimport --host=172.17.0.2:27017 --db new_york --collection restaurants ../restaurants.json