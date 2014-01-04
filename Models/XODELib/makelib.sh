gcc -c XODEParser.cpp -o XODEParser.o
ar  rcs libXODEParser.a   XODEParser.o
cp libXODEParser.a ../../Robots/Buggy_V01/
cp libXODEParser.a ../../Robots/Buggy_V02/
cp libXODEParser.a ../../Robots/Snake/
