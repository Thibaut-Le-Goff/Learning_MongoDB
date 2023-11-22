#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

// gcc -o test test.c $(pkg-config --cflags --libs libmongoc-1.0) && ./test > test.txt

int main(void) {
    mongoc_client_t *client = NULL;
    bson_error_t error = {0};
    mongoc_database_t *database = NULL;
    bson_t *command = NULL;
    bson_t reply = BSON_INITIALIZER;
    const bson_t *doc;
    bson_t *query;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;

    char *str;
    int return_state = 0;
    bool ok = true;
    int64_t count;


    // Initialize the MongoDB C Driver.
    mongoc_init();

    //client = mongoc_client_new("mongodb://127.0.0.1:27017");
    client = mongoc_client_new("mongodb://172.17.0.2:27017");

    if (!client) {
        fprintf(stderr, "Failed to create a MongoDB client.\n");

        return_state = 1;
        goto cleanup;
    }

    // Get a handle on the "admin" database.
    //database = mongoc_client_get_database(client, "admin");
    database = mongoc_client_get_database(client, "user");
    if (!database) {
        fprintf(stderr, "Failed to get a MongoDB database handle.\n");
        return_state = 1;
        goto cleanup;
    }

    collection = mongoc_client_get_collection(client, "new_york", "restaurants");

    query = bson_new();
    //BSON_APPEND_UTF8(query, "name", "Harriet'S Kitchen");
    //BSON_APPEND_UTF8(query, "grade", "A");
    BSON_APPEND_UTF8(query, "cuisine", "American");
    //BSON_APPEND_UTF8(query, "score", "12");
    BSON_APPEND_UTF8(query, "borough", "Manhattan");

    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    //doc = bson_new_from_json((const uint8_t *) "{\"name\": \"Harriet'S Kitchen\"}", -1, &error);
    //doc = bson_new_from_json((const uint8_t *) "{\"score\": \"12\"}", -1, &error);
    // "name" : "Harriet'S Kitchen",

    while (mongoc_cursor_next (cursor, &doc)) {
        str = bson_as_canonical_extended_json(doc, NULL);
        printf ("%s\n", str);
        bson_free (str);
    }

    //doc = bson_new_from_json((const uint8_t *) "{}", -1, &error);
    //doc = bson_new_from_json((const uint8_t *) "{}", -1, &error);
    //doc = bson_new_from_json((const uint8_t *) "{\"score\": \"12\"}", -1, &error);

    /*
    count = mongoc_collection_count_documents(collection, doc, NULL, NULL, NULL, NULL);

    if (count < 0) {
        fprintf(stderr, "%s\n", error.message);
    } else {
        printf("%" PRId64 "\n", count);
    }
    */

    //printf("new_york : %s\n", collection);

    // Ping the database.
    command = BCON_NEW("ping", BCON_INT32(1));
    ok = mongoc_database_command_simple(database, command, NULL, &reply, &error);

    if (!ok) {
        fprintf(stderr, "error: %s\n", error.message);
        return_state = 1;
        goto cleanup;
    }
    bson_destroy(&reply);

    printf("Pinged your deployment. You successfully connected to MongoDB!\n");

    // Perform cleanup.
    cleanup:
        mongoc_client_destroy(client);
        mongoc_database_destroy(database);
        bson_destroy(command);
        //bson_destroy(reply);
        //bson_destroy(doc);
        bson_destroy(query);
        mongoc_collection_destroy(collection);
        mongoc_cursor_destroy(cursor);

        mongoc_cleanup();

        return return_state;
}