import ROOT
from ROOT import *
from ROOT import TFile, TTree


gBenchmark.Start('MR2')
e = 255
event = 0
bytes = 0
ampl = 0
adc_num = 0

f = TFile( 'test1.root', 'recreate' )
t = TTree( 't', 'tree' )
print 't,f created'
t.Branch('ampl', ampl, 'ampl/I')
t.Branch('adc_num', adc_num, 'adc_num/I')
print "branches created"

print t

with open('R7_0c', 'rb') as f:
    while True:
        adc0 = f.read(1)
        if not adc0: break
        adc = ord(adc0)
        bin_slot = ord(f.read(1))
        adc_low = ord(f.read(1))
        adc_high = ord(f.read(1))
        bytes =  bytes + 1
        if e in (adc,bin_slot):
            #print 'event'
            event = event + 1
        ampl = (256*(adc_low))+adc_low
        adc_num = 32*(bin_slot-1)+adc
        #print adc, bin_slot,adc_low,adc_high
        #print adc_num, ampl
        t.Fill()

print event,bytes*4
f.Write()
f.Close()
print "write and close done"


gBenchmark.Show('MR2')
