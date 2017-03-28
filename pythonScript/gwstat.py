


import sys
import socket
import subprocess
import struct
import random
import base64

from struct import *

argvs = sys.argv
argc = len(argvs)
print argvs
if (argc < 3):
    print 'Usage: # python %s filename' % argvs[0]
    quit()

print 'ip    : %s ' % argvs[1]
print 'port  : %s ' % argvs[2]
print 'argvs1: %s ' % argvs[3]

# change your gateway ID(from 0xA8 to 0xD4)
head = chr(1) \
+ chr(random.randint(0,255)) \
+ chr(random.randint(0,255)) \
+ chr(0) \
+ chr(0xA8) \
+ chr(0xAA) \
+ chr(0xAA) \
+ chr(0xFF) \
+ chr(0xFF) \
+ chr(0xAA) \
+ chr(0xAA) \
+ chr(0xD4)


if (argvs[3] == "stat"):
    stat = "{\"stat\":{" \
      + "\"time\":" + subprocess.check_output(["date", "+\"%Y-%m-%d %H:%M:%S GMT"])[0:24] + "\"," \
      + "\"lati\":139.6315," \
      + "\"long\":35.479," \
      + "\"alti\":0," \
      + "\"rxnb\":0," \
      + "\"rxok\":0," \
      + "\"rxfw\":0," \
      + "\"ackr\":0," \
      + "\"dwnb\":0," \
      + "\"txnb\":0," \
      + "\"pfrm\":\"Dragino LG01-JP\"," \
      + "\"mail\":\"asuzuki@openwave.co.jp\"," \
      + "\"desc\":\"\"" \
      + "}}"
    head+=stat
else:
    rxpk = "{\"rxpk\":[{" \
      + "\"tmst\":" + subprocess.check_output(["date", "+%s"])[0:10] + ",".replace('\n','') \
      + "\"chan\":0," \
      + "\"rfch\":0," \
      + "\"freq\":" + argvs[5] + "," \
      + "\"stat\":1," \
      + "\"modu\":\"LORA\"" + "," \
      + "\"datr\":\"SF7" \
      + "BW125\""  + ","\
      + "\"codr\":\"4/5\"," \
      + "\"lsnr\":9," \
      + "\"rssi\":" + argvs[3] + "," \
      + "\"size\":" + argvs[4] + "," \
      + "\"data\":\""
    
    f = open('/mnt/mtdblock3/data/bin','rb')
    data1 = f.read()
    f.close()
    rxpk+=base64.b64encode(data1)

    rxpk+="\"}]}"
    head+=rxpk

print(head)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(bytes(head), (argvs[1], int(argvs[2])))
sock.close()

