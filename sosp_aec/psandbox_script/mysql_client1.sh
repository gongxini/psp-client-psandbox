#!/bin/bash

sudo ${PSP_DIR}/build/src/c++/apps/client/client --config-path ${PSP_DIR}/sosp_aec/configs/client_mysql_case1_interference.yml --label test --ip 192.168.10.10 --port 6789 --max-concurrency -1 --sample -1 --collect-logs 1 --outdir client0 --req-offset 12
sudo ${PSP_DIR}/build/src/c++/apps/client/client --config-path ${PSP_DIR}/sosp_aec/configs/client_mysql_case1_normal.yml --label test --ip 192.168.10.10 --port 6789 --max-concurrency -1 --sample -1 --collect-logs 1 --outdir client0 --req-offset 12

sudo ${PSP_DIR}/build/src/c++/apps/client/client --config-path ${PSP_DIR}/sosp_aec/configs/client_mysql_case2.yml --label test --ip 192.168.10.10 --port 6789 --max-concurrency -1 --sample -1 --collect-logs 1 --outdir client0 --req-offset 14