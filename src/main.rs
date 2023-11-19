use mongodb::{Client, options::ClientOptions};

async fn connect() {
    // Parse a connection string into an options struct.

    let mut client_options: ClientOptions = ClientOptions::parse("mongodb://172.0.0.2:27017").await.unwrap();
    
    println!("client option app name : {:?}", client_options.app_name);

    
    // Manually set an option.
    client_options.app_name = Some("My App".to_string());
    println!("client option app name : {:?}", client_options.app_name);

    println!("client option data base: {:?}", client_options.default_database);

    
    // Get a handle to the deployment.
    let client: Client = Client::with_options(client_options).unwrap();

    println!("client data base: {:?}", client.default_database());

    /*
    // List the names of the databases in that deployment.
    for db_name in client.list_database_names(None, None).await.unwrap() {
        println!("{:?}", db_name);
    }
    */
}

#[tokio::main]
async fn main() {

    connect().await;
}