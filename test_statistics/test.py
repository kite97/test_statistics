#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess

filename1 = "outtest_long.wav"
retcode1 = subprocess.check_output(["./statistics", filename1])
print(filename1 +":")
if "Data redundancy" in str(retcode1):
    print("Pass")
else:
    print("error")

filename2 = "outtest_short.wav"
retcode2 = subprocess.check_output(["./statistics", filename2])
print(filename2 +":")
if "Missing data" in str(retcode2):
    print("Pass")
else:
    print("error")

filename3 = "1.wav"
retcode3 = subprocess.check_output(["./statistics", filename3])
print(filename3 +":")
if "It's a correct wave file" in str(retcode3):
    print("Pass")
else:
    print("error")

filename4 = "1.mp3"
retcode4 = subprocess.check_output(["./statistics", filename4])
print(filename4 +":")
if "It's a correct mp3 file" in str(retcode4):
    print("Pass")
else:
    print("error")

filename5 = "2.mp3"
retcode5 = subprocess.check_output(["./statistics", filename5])
print(filename5 +":")
if "It's a correct mp3 file" in str(retcode5):
    print("Pass")
else:
    print("error")

filename6 = "2error_mode.amr"
retcode6 = subprocess.check_output(["./statistics", filename6])
print(filename6 +":")
if "There is a bad frame or the mode of sampling rate error" in str(retcode6):
    print("Pass")
else:
    print("error")

filename7 = "3change.awb"
retcode7 = subprocess.check_output(["./statistics", filename7])
print(filename7 +":")
if "The mode of sampling rate has changed." in str(retcode7):
    print("Pass")
else:
    print("error")

filename8 = "2_error_mode.amr"
retcode8 = subprocess.check_output(["./statistics", filename8])
print(filename8 +":")
if "There is a bad frame or the mode of sampling rate error" in str(retcode8):
    print("Pass")
else:
    print("error")

filename9 = "no_header.amr"
retcode9 = subprocess.check_output(["./statistics", filename9])
print(filename9 +":")
if "The header error" in str(retcode9):
    print("Pass")
else:
    print("error")

filename10 = "1.amr"
retcode10 = subprocess.check_output(["./statistics", filename10])
print(filename10 +":")
if "It's a correct amr or awb file" in str(retcode10):
    print("Pass")
else:
    print("error")

filename11 = "3-CN.awb"
retcode11 = subprocess.check_output(["./statistics", filename11])
print(filename11 +":")
if "There is a Comfort Noise Frame" in str(retcode11):
    print("Pass")
else:
    print("error")

filename12 = "2_SID.amr"
retcode12 = subprocess.check_output(["./statistics", filename12])
print(filename12 +":")
if "There is a SID Frame" in str(retcode12):
    print("Pass")
else:
    print("error")

filename13 = "2.amr"
retcode13 = subprocess.check_output(["./statistics", filename13])
print(filename13 +":")
if "It's a correct amr or awb file" in str(retcode13):
    print("Pass")
else:
    print("error")

filename14 = "3.awb"
retcode14 = subprocess.check_output(["./statistics", filename14])
print(filename14 +":")
if "It's a correct amr or awb file" in str(retcode14):
    print("Pass")
else:
    print("error")
