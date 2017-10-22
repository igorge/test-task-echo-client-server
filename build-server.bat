@echo off

cd server


rem rmdir buildwin /s /q

mkdir buildwin

cd buildwin

cmake ../.

cmake --build .
