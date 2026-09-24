#!/bin/bash

declare -a upstreams
upstreams=(opm-common)

declare -A upstreamRev
upstreamRev[opm-common]=master

# Downstreams and revisions
declare -a downstreams
downstreams=(opm-simulators
             opm-upscaling)

declare -A downstreamRev
downstreamRev[opm-simulators]=master
downstreamRev[opm-upscaling]=master

# Fetch opm-common before loading its shared Jenkins helpers.
source "$WORKSPACE/jenkins/checkout-opm-common.sh"

source $WORKSPACE/deps/opm-common/jenkins/build-opm-module.sh

parseRevisions
printHeader opm-grid

clone_repositories opm-grid

# Setup opm-data
if grep -q "with downstreams" <<< $ghprbCommentBody
then
    source $WORKSPACE/deps/opm-common/jenkins/setup-opm-tests.sh
fi

build_module_full opm-grid
