navigator.geolocation.getCurrentPosition(
function(a){m='Latitude:'+a.coords.latitude+'\nLongitude:'+a.coords.longitude;
alert(m);
},
function(){},
{enableHighAccuracy:true,maximumAge:0})
