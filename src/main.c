//#include "../mongo-c-driver-1.25.4/src/libbson/src/bson/bson.h"
#include <bson/bson.h>
//#include "../mongo-c-driver-1.25.4/src/libmongoc/src/mongoc/mongoc.h"
#include <mongoc/mongoc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h>

#include "../include/partie_1/interrogez_mongodb.h"

// gcc -o test test.c $(pkg-config --cflags --libs libmongoc-1.0) && ./test > test.txt
// mongoimport --host=172.17.0.2:27017 --db new_york --collection restaurants restaurants.json

/**/
int main() {
    int return_state = 0;

    // Initialize the MongoDB C Driver.
    mongoc_init();

    //mongoc_client_t *client = mongoc_client_new("mongodb://172.17.0.2:27017");
    mongoc_client_t *client =  mongoc_client_new ("mongodb://admin:mot_de_sel_passe@172.17.0.2:27017/?authSource=admin");
    
    if (!client) {
        fprintf(stderr, "Failed to create a MongoDB client.\n");

        return_state = 1;
        goto cleanup_client;
    }

    // Get a handle on the "admin" database.
    mongoc_database_t *database = mongoc_client_get_database(client, "admin");
    if (!database) {
        fprintf(stderr, "Failed to get a MongoDB database handle.\n");
        return_state = 1;
        goto cleanup_database;
    }


    // Interrogez vos données avec MongoDB
    filtrage_et_projection(client);
    

    // Perform cleanup.
    cleanup_database:
        mongoc_database_destroy(database);

    cleanup_client:
        mongoc_client_destroy(client);
        mongoc_cleanup();

    return return_state;
}

/*
{
    "address": {
        "building": "19", 

        "coord":{
            "type":"Point", 
            "coordinates" : [-73.9919172, 40.7224662]
        }, 

        "street": "Stanton Street", 
        "zipcode": "10002"
    }, 

    "borough": "Manhattan", 
    "cuisine": "American", 

    "grades": [{

        "date": {
            "$date": 1403827200000
        }, 

        "grade": "A", 
        "score": 11
    }], 

    "name": "Cocktail Bodega", 
    "restaurant_id": "50010278"
}

    list:             Show the source code.
    print <variable>: Print the value of a variable.
    backtrace or bt:  Display a backtrace of the call stack.
    step or s:        Execute a single instruction, stepping into function calls.
    next or n:        Execute a single instruction, stepping over function calls.
    continue or c:    Continue running until the next breakpoint.

*/