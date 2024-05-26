#!/bin/sh
./eot -g > eot.csv &&
octave --persist graph.m
