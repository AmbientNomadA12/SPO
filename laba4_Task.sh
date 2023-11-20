#!/bin/bash
ps -U $1 | awk '{print $1, $4}'