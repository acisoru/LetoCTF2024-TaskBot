#!/bin/sh
socat tcp-listen:1557,reuseaddr,fork exec:"./cookies"

