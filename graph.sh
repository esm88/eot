#!/bin/bash
./eot -g > eot.csv &&
octave --persist graph.m
