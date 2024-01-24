db.createUser({
    user: "admin",
    pwd: "mot_de_sel_passe",
    roles: [{ role: "userAdminAnyDatabase", db: "admin" }],
    //mechanisms: ["SCRAM-SHA-256"],
    passwordDigestor: "server"
});