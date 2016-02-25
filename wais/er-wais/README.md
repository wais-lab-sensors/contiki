This is a demo of Coap over 6LowPAN for Zolertia z1 nodes with an SHT25 sensor.
make sure you compile with TARGET=z1-wais

the .well_known resource is simplified to "Wais sensor readings" title

to get the data GET / and you should see something like
{"reading":{"temperature":15.95,"humidity":57.35,"battery":2350}}

Kirk Martinez
University of Southampton
