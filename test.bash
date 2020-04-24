#!/bin/bash

if [ $TRAVIS_EVENT_TYPE = "cron" ]; then
echo "Test  : Nightly"
ctest -D Nightly
elif [ $TRAVIS_EVENT_TYPE = "push" ]; then
echo "Test: Continuous"
ctest -D Continuous
else
echo "Test: Experimental"
ctest -D Experimental
fi

