#!/bin/bash
A=$(dirname $0); cat $A/head.h $A/models.h $2 > $1
