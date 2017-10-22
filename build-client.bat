@echo off

cd client


rem rmdir buildwin /s /q

mkdir buildwin

cd buildwin

cmake ../.

cmake --build .
