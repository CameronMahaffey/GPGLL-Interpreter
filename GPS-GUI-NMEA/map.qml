import QtQuick 2.0
import QtPositioning 5.15
import QtLocation 5.15

Rectangle {
    id: window
    property double initial_lat: 35.2688
    property double initial_lon: -81.5352

    Plugin {
        id: mapPlugin
        name: "osm"

        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "http://maps-redirect.qt.io/osm/5.6/"
        }
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(initial_lat, initial_lon);
        zoomLevel: 15
    }

    function setCenter(lat, lon){
        mapView.clearMapItems()
        initial_lat = lat
        initial_lon = lon
        var Component = Qt.createComponent("qrc:///marker.qml")
        var item = Component.createObject(window, {
                                          coordinate: QtPositioning.coordinate(lat, lon)
                                          })
        mapView.addMapItem(item)
    }
}
