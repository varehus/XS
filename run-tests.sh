#!/usr/bin/env sh

if [ ! -f "build/xs" ]; then
    echo "Error: 'xs' not found."
    exit 1
fi

./build/xs tests/xs_tests.xs
