g++ -o buggy buggy.cpp -I../../Engine/include/ -I../../Models/XODELib/ -I. ../../Engine/ode/src/.libs/libode.a ../../Models/XODELib/libXODEParser.a ../../Engine/drawstuff/src/.libs/libdrawstuff.a  -lGL -lX11 -lGLU -ltinyxml -L. -lXODEParser