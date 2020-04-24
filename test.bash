#!/bin/bash

if [ $TRAVIS_EVENT_TYPE = "cron" ]; then
echo "Test  : Nightly"
ctest -D Nightly
else if [ $TRAVIS_EVENT_TYPE = "push" ]
echo "Test: Continuous"
ctest -D Continuous
else
echo "Test: Experimental"
ctest -D Experimental
fi

