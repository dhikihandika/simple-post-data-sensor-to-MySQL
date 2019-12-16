// -------------------------------------------------------------------------------------------------------------------------------------------------------//
// ---------------------------------------------------------------- Create Web Server --------------------------------------------------------------------//
// ----------------------------------------------------------------    RESTful APIs   --------------------------------------------------------------------//
// -------------------------------------------------------------------------------------------------------------------------------------------------------//
// ------------------------------------------------------ //    
// date create : 11, Septemmber 2019                                                                
// author      : dhikihandika                                                                       
// email       : dhikihandika36@gmail.com                       
// ------------------------------------------------------ //    

// -------------------------------------------------------|         Initialize Program          |-------------------------------------------------------- //
const mysql = require('mysql');                                                     // Acces module MySQL database
const express = require('express');                                                 // Acces module express.js 
var app = express();                                                                // Define variable 'app' its express function
const bodyParser = require('body-parser');                                          // Acces module body-parser

app.use(express.json());                                                            // 
app.use(bodyParser.json());                                                         //

// Make server can be get with different origin/domain CORS (Cross Origin Resource Sharing)
app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");                                                 // Acces database can be get in different origin (domain)
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");   // Set header in Access-Contorl-Allow Cross Origin Resource Sharing
    res.header("Access-Control-Allow-Methods", "DELETE, GET, POST, PUT");                           // Set methode in Access-Contorl-Allow Cross Origin Resource Sharing
    next();
});     

var conn = mysql.createConnection({                                                 // Function to create connection with database MySQL
    host: 'localhost',                                                              // Default host
    user: 'root',                                                                   // Default user
    password: '',                                                                   // Default pass
    database: 'vawt'                                                                // "reports_db" database
});
// callback function connection to database
conn.connect(function(err){                                                         // Connection 
    if(err) throw err.code + err.fatal;                                             // Error handling, if error == err.code : String, MySQL Server Error  
                                                                                    //                          == err.fatal: Boolean, error is not from a MySQL
    console.log("Conected to database ");                                           // if not error show on console ("...")
});


// ------------------------------------------------------- Get all database by request URL ---------------------------------------------------------------// 
// URL = localhost:8080/api/payload/get                          
app.get('/api/payload/get', (req,res)=>{                                            // Define route to handler HTTPRequest GET all data (path APIs, callback function)
    let sql = "SELECT * FROM payload";                                              // "sql" it is variable query to get database 
    conn.query(sql, function(err, rows, fields){                                    // Query connection and callback function 
        if(err) throw err.code + err.fatal;                                         // Error handling 
        let gebt = "doi";
        let name = JSON.stringify(gebt);
        let data = JSON.stringify(rows);                                            // Parsing data JavaScript object to string
        let objName = ("{" +name+" :" + data + "}");
        res.send(objName);                                                          // Response open rows data MySQL

        console.log("Response Success :  \n" + objName);                            // Show on console "Response Success :  \n" with value of data
    });
});


// Get spesific database from request URL
// URL = localhost:8080/api/payload/get/(id)
app.get('/api/payload/get/last10', (req,res)=>{                                     // Define route to handler HTTPRequest GET spesific data (path APIs, callback function)
    let sql = "SELECT * FROM `payload` ORDER BY no DESC LIMIT 10";                  // "sql" its varible query to get last 10 rows database
    conn.query(sql, [req.params.id], (err, rows, fields)=>{                         // Query Connection and callback function
        if(err) throw err.code + err.fatal;                                         // Error handling 
        let gebt = "doi";
        let name = JSON.stringify(gebt);
        let data = JSON.stringify(rows);                                            // Parsing data Javascript object to string
        let objName = ("{" + name+ ":" + data + "}");
        res.send(objName);                                                          // response open rows data MySQL
        console.log("Response Success :  \n"+ objName);                             // Show on console "Response Success :  \n" with value of data
    });
});

// Get spesific database from request URL
// URL = localhost:8080/api/payload/get/(id)
app.get('/api/payload/get/last5', (req,res)=>{                                     // Define route to handler HTTPRequest GET spesific data (path APIs, callback function)
    let sql = "SELECT * FROM `payload` ORDER BY no DESC LIMIT 5";                  // "sql" its varible query to get last 10 rows database
    conn.query(sql, [req.params.id], (err, rows, fields)=>{                         // Query Connection and callback function
        if(err) throw err.code + err.fatal;                                         // Error handling 
        let gebt = "doi";
        let name = JSON.stringify(gebt);
        let data = JSON.stringify(rows);                                            // Parsing data Javascript object to string
        let objName = ("{" + name+ ":" + data + "}");
        res.send(objName);                                                          // response open rows data MySQL
        console.log("Response Success :  \n"+ objName);                             // Show on console "Response Success :  \n" with value of data
    });
});

// ------------------------------------------------------ Post data to database use URL ------------------------------------------------------------------//
// URL = localhost:8080/api/payload/post
app.post('/api/payload/post', (req, res) => {                                       // Define route to handler HTTPRequest POST data (path APIs, callback function) 
    id_data = req.body.id,
    tempCelcsius = req.body.valueC,                                                 // "valueC" its object JSON value from client
    tempFahrenheit = req.body.valueF                                                // "valueF" its object JSON value from client
    let sql = "INSERT INTO payload (id_data, tempCelcsius, tempFahrenheit) VALUES (?,?,?)";       // "sql" its varible query to create database      
    conn.query(sql, [id_data, tempCelcsius, tempFahrenheit], (err, rows, fields) => {             // Query connection 
        if(err) throw err.code + err.fatal;                                         // Error handling
          res.send(rows);                                                           // When not error send (rows)
          console.log("Success post data");                                         // Show on console "Success post data"
     });
 });


// -------------------------------------------------------- Port listen = where program running ----------------------------------------------------------//
var port=8080;                                                                      // Declare computer port listen
app.listen(port, ()=> console.log('Server running on port '+port));                 // listen port function 


// ---- ATTENTION ???????????????????
// - Before you running this program make sure MySQL database is running and has been make a simple database
