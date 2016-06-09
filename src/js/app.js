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
    var settings = JSON.parse(decodeURIComponent(e.response));
    
    if (settings) {
      
      localStorage.setItem("rustic_slider_config", JSON.stringify(settings));
      
      //Send to Pebble, persist there
      Pebble.sendAppMessage(
          {
            "DIRECTION": settings.direction
          },
        function(e) {
          console.log("Sending settings data... DIRECTION");
        },
        function(e) {
          console.log("Settings feedback failed!");
        }
      );
    }
  }
);