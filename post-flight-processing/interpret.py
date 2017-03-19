"""
ALL ARE uint16
BYTES           DATATYPE
--------------------------------------
0-16        16byte header marker
16-17       image number
17-18       number of stars detected
19-20       previous star flag (0 if previous stars succesfully read in, 1 otherwise)
20-21       stars flag (0 if stars are matched, 1 otherwise)
22-23       x drift in milliarcsec
24-25       y drift in milliarcsec
26-27       z drift in milliarcsec
28-29       new stars flag (0 if prev stars are kept, 1 otherwise)
29-30       number of postage stamp
31-48       the x pixel positions of the the center of the 9 postage stamps in order
49-66       the y pixel positions of the the center of the 9 postage stamps in order
67-266      the postage stamp 1
267-466     the postage stamp 2
467-666     the postage stamp 3
667-866     the postage stamp 4
867-1066    the postage stamp 5
1067-1266   the postage stamp 6
1267-1466   the postage stamp 7
1467-1666   the postage stamp 8
1667-1866   the postage stamp 9

for total:
header : 16-66
data : 67:1866

after marker removal
header : 0-50
data : 51:1851
"""

import numpy
import cv2
from bitstring import BitArray, BitStream
import time


class Display(object):
    def __init__(self,Datum,dims=(1024,1024),save=True):
        self._Datum = Datum
        self._rows = dims[0]
        self._cols = dims[1]
        cv2.namedWindow("CSTARS",)

    def reconstructImage(self,centroids,imageStack):
        template = np.zeros()


    def display(self,img):
        cv2.imshow

    def writeFrame(self,frame):



class Datum(object):
    def __init__(self,data):
        self._data = None
        rawHeader = self._data[16:31]
        self._header = {"ImageNumber":self.fromBytes( rawHeader[0:1] ),
                            "StarNumber":self.fromBytes( rawHeader[2:3] ),
                            "PreviousStarFlag":self.fromBytes( rawHeader[4:5] ),
                            "MatchedStarFlag":self.fromBytes( rawHeader[6:7] ),
                            "xDrift":self.fromBytes( rawHeader[8:9] ),
                            "yDrift":self.fromBytes( rawHeader[10:11] ),
                            "zDrift":self.fromBytes( rawHeader[12:13] ),
                            "newStarFlag":self.fromBytes( rawHeader[14:15] ),
                            "NumberOfPostageStamps":self.fromBytes( rawHeader[16:17] )
                            }
        # rawCentroids[]
        # self._centroids

        rawImages = self._data[66:]
        self._postageStamps = self.separateImages(rawImages)


    def fromBytes(self,input1):
        #returns unsigned int of raw bytes
        return int.from_bytes(input1, byteorder="big", signed=False)

    def byteToImg(self,input1):
        byteIndicies = np.arange(0,99) * 2
        img = np.zeros( (10,10), dtype=np.uint16)
        for imgIndex,byteIndex in enumerate( range(byteIndicies) ):
            img.flat[imgIndex] = self.from_bytes( input1[byteIndex : (byteIndex+2)] )
        return img

    # def btyeToCentroids(self,input1):
    #     byteIndicies = np.arange(0,99) * 2
    #     xArray = []
    #     yArray = []


    def separateImages(self,input1):
        images = []
        for i in range(self._header["NumberOfPostageStamps"]):
            stamp = input1[i*200:i*200+200]
            img = self.byteToImg(stamp)
            images.append(img)
        return images


class FifoReader(object):

    def __init__(self,fifoPath,marker,autoDebug=True):
        self._fifoPath = fifoPath
        self._marker = marker
        self._fifo = open(self._fifoPath,"rb")

    def readFifo(self):
        data = self._fifo.read()
        return data

    def parse(self,rawData):
        #searching for marker
        markerLocations = [pos for pos, char in enumerate(rawData) if char == self._marker]

        if len(markerLocations) == 0:
            self._dataBuffer = self._dataBuffer + readFifo
            return 0;
        elif (len(markerLocations) > 2 and (markerLocations[0] == 0) ): #if header is at beginning
            singleOutputData = self._dataBuffer[ markerLocations[0] : ( markerLocations[1]-1 ) ]
            self._dataBuffer = self._dataBuffer[ ( markerLocations[1]-1 ) : ]

        header = self._dataBuffer[16,66]
        data = self._dataBuffer[67:]






if __name__ == "__main__":
    #this should correspond to "ritcstars" --> a ascii-readable marker
    # marker = "\x72\x69\x74\x63\x73\x74\x61\x72\x73"
    marker = b"ritcstars"
    DataReader = FifoReader("output.fifo",marker)
