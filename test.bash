#!/bin/bash

if [ $TRAVIS_EVENT_TYPE = "cron" ]; then
echo "Test  : Nightly"
ctest -D Nightly
else
echo "Test: Experimental"
ctest -D Experimental
fi

