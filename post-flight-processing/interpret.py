import numpy
import openCV



class Datum(object):
    def __init__(self):



class BinaryData(object):

    def __init__(self,filename,marker):
        self._filename = filename
        self._marker = marker


    def readFifo(self):
        with open(self._filename,"rb" as file:
            return file.read();

    def searchForMarker(self,rawData):









if __name__ == "__main__":
    marker = "\x72\x69\x74\x63\x73\x74\x61\x72\x73"
