#!usr/bin/python

import smbus
import time

bus = smbus.SMBus(1)
slave1 = 0x38

pin_adres = input('Pin No Giriniz: ')
durum = input('Durum Giriniz: ')
print " Veri Gonderiliyor..."

bus.write_byte_data(slave1, pin_adres, durum)
time.sleep(0.1)

print "Veri Okunuyor..."
bus.write_byte(slave1, pin_adres)
time.sleep(0.1)
retValue = bus.read_byte(slave1);

print "     Pin %s = %s" % (pin_adres, retValue)