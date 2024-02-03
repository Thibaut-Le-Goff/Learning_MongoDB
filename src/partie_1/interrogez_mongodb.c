#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void filtrage_et_projection(mongoc_client_t *client) {

    // selection de la collection restaurant de la base de données new_york
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "new_york", "restaurants");

    // creation de la requête, on veut...
    bson_t *query = BCON_NEW("borough", BCON_UTF8("Brooklyn"),
                            // ...dans l'arrondissement de Brooklyn...
                            "cuisine", BCON_UTF8("Italian"),
                            // ...uniquement les restaurant italiens...
                            "address.street", BCON_UTF8("5 Avenue"),
                            // ...de la cinquième avenu...
                            "name", "{", "$regex", BCON_UTF8("pizza"), "$options", BCON_UTF8("i"), "}");
                            //regex, "$regex", "pizza", "i"
                            // ...où il y a le mot "pizza" dans le nom...
    
    // ...voir uniquement...
    bson_t *projection = BCON_NEW("name", BCON_INT32(1),
                                // ...les noms des restaurants...
                                "grades.score", BCON_INT32(1),
                                // ...leurs notes...
                                "_id", BCON_INT32(false));
                                // ...en cachant leur IDs.

    bson_t *opts = BCON_NEW("projection", BCON_DOCUMENT(projection));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (collection, query, opts, NULL);

    const bson_t *doc = NULL;
    while (mongoc_cursor_next(cursor, &doc)) {
        /*
        if (doc != NULL) 
        {
            char *str = bson_as_canonical_extended_json(doc, NULL);
            printf("%s\n", str);
            bson_free(str);
        }*/
        
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }
    
    // nettoyage
    mongoc_cursor_destroy(cursor);
    bson_free((char*)doc);
    mongoc_collection_destroy(collection);
    bson_destroy(query);
    bson_destroy(projection);
    bson_destroy(opts);
}