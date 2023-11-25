#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

// gcc -o test test.c $(pkg-config --cflags --libs libmongoc-1.0) && ./test > test.txt

int main(void) {
    bson_error_t error = {0};
    bson_t reply = BSON_INITIALIZER;

    int return_state = 0;
    bool ok = true;
    int64_t count;


    // Initialize the MongoDB C Driver.
    mongoc_init();

    //client = mongoc_client_new("mongodb://127.0.0.1:27017");
    mongoc_client_t *client = mongoc_client_new("mongodb://172.17.0.2:27017");
    if (!client) {
        fprintf(stderr, "Failed to create a MongoDB client.\n");

        return_state = 1;
        goto cleanup_client;
    }

    // Get a handle on the "admin" database.
    mongoc_database_t *database = mongoc_client_get_database(client, "user");
    if (!database) {
        fprintf(stderr, "Failed to get a MongoDB database handle.\n");
        return_state = 1;
        goto cleanup_database;
    }

    mongoc_collection_t *collection = mongoc_client_get_collection(client, "new_york", "restaurants");

    bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "borough", "Brooklyn");
    BSON_APPEND_UTF8(query, "cuisine", "Italian");
    BSON_APPEND_UTF8(query, "address.street", "5 Avenue");

    //const bson_value_t *value;
    //value = bson_iter_value("/pizza/i");

    //BSON_APPEND_VALUE(query, "name", value);
    //BSON_APPEND_UTF8(query, "name", '/"pizza"/i');
    //BSON_APPEND_VALUE(query, "name", '/"pizza"/i');
    //BSON_APPEND_UTF8(query, "name", /"pizza"/i);

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    const bson_t *doc = NULL;
    char *str;
    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }

    // Perform cleanup.
    bson_free((char*)doc);
    mongoc_collection_destroy(collection);
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);

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
*/