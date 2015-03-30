Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    var configurationSTR = localStorage.getItem("rustic_slider_config") || '{"direction": 0}';
    var configuration = JSON.parse(configurationSTR);
    
    //Load the remote config page
    Pebble.openURL("http://codecorner.galanter.net/pebble/rustic_slider_config.htm?direction=" + configuration.direction);
    
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    var configurationSTR = JSON.stringify(configuration);
    console.log("Configuration window returned: " + configurationSTR);
    
    if (configurationSTR != '{}') {
      
      localStorage.setItem("rustic_slider_config", configurationSTR);
 
      //Send to Pebble, persist there
      Pebble.sendAppMessage(
          {
            "KEY_DIRECTION": configuration.direction
          },
        function(e) {
          console.log("Sending settings data...");
        },
        function(e) {
          console.log("Settings feedback failed!");
        }
      );
    }
  }
);