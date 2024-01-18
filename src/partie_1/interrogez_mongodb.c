#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void filtrage(mongoc_client_t *client) {

    // selection de la collection 
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "new_york", "restaurants");


    // création de la requète
    /*bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "borough", "Brooklyn");
    BSON_APPEND_UTF8(query, "cuisine", "Italian");
    BSON_APPEND_UTF8(query, "address.street", "5 Avenue");


    // utilisation de regex pour filtrer les
    // contenant pizza
    bson_t *regex = bson_new();
    BSON_APPEND_REGEX(regex, "$regex", "pizza", "i");
    BSON_APPEND_DOCUMENT(query, "name", regex);
    // "name" car le regex s'applique aux noms 
    */

    /*
    bson_t *filter;
    bson_t *opts;
    mongoc_read_prefs_t *read_prefs;
    
    filter = BCON_NEW ("foo", BCON_INT32 (1));

    // Include "field_name_one" and "field_name_two" in "projection", omit
    // others. "_id" must be specifically removed or it is included by default.
    
    opts = BCON_NEW ("projection", "{",
                        "field_name_one", BCON_BOOL (true),
                        "field_name_two", BCON_BOOL (true),
                        "_id", BCON_BOOL (false),
                    "}",
                    "tailable", BCON_BOOL (true),
                    "awaitData", BCON_BOOL (true),
                    "sort", "{", "bar", BCON_INT32 (-1), "}",
                    "collation", "{",
                        "locale", BCON_UTF8("en_US"),
                        "caseFirst", BCON_UTF8 ("lower"),
                    "}");

    read_prefs = mongoc_read_prefs_new (MONGOC_READ_SECONDARY);

    cursor = mongoc_collection_find_with_opts (collection, filter, opts, read_prefs);
    */
    bson_t *filter;
    bson_t *projection = BCON_NEW("name", BCON_INT32(1));
    bson_t *opts = BCON_NEW("projection", BCON_DOCUMENT(projection));
    
    filter = BCON_NEW ("foo", BCON_INT32 (1));

    // Include "field_name_one" and "field_name_two" in "projection", omit
    // others. "_id" must be specifically removed or it is included by default.

    bson_t *query = BCON_NEW("borough", BCON_UTF8("Brooklyn"),
                            "cuisine", BCON_UTF8("Italian"),
                            "address.street", BCON_UTF8("5 Avenue"),
                            "name", "{", "$regex", BCON_UTF8("pizza"), "$options", BCON_UTF8("i"), "}");


    //mongoc_read_prefs_t *read_prefs = mongoc_read_prefs_new (MONGOC_READ_SECONDARY);

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (collection, query, opts, NULL);


    //bson_t *projection = BCON_NEW("name", "{", "$exists", BCON_BOOL(true), "}");
    //BSON_APPEND_UTF8(query, "name", BCON_INT32(1));
    //bson_t *projection = BCON_NEW("name", BCON_INT32(1));
    //bson_t *projection = bson_new();
    //BSON_APPEND_INT32(projection, "name", 1);

    // interrogation de la BDD
    //mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    const bson_t *doc = NULL;
    //char *str;
    while (mongoc_cursor_next (cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }
    
    // nettoyage
    mongoc_collection_destroy(collection);
    bson_destroy(query);
    //bson_destroy(regex);
    bson_destroy(projection);
    bson_destroy(filter);
    bson_destroy(opts);
    mongoc_cursor_destroy(cursor);
    bson_free((char*)doc);
}