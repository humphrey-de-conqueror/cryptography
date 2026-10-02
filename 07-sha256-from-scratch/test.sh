#!/bin/bash

for file in test-0 test-55 test-56 test-63 test-64 test-65
do
	echo "Testing $file..."

	./sha256 "$file" > my-hash
	sha256sum "$file" | cut -d ' ' -f 1 > reference-hash

	if diff -q my-hash reference-hash > /dev/null
	then
		echo "PASS"
	else
		echo "FAIL"
		diff my-hash reference-hash
	fi

	echo
done