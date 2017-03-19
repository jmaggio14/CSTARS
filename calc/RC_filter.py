import numpy


class RCFilter(object):


	def __init__(self,fc,voltage,current,mode="current-limited"):
		self._filter_frequency = filter_frequency
		self._voltage = voltage
		self._current = current
		self._mode = mode
		self._RC = None


	def calc(self):
		if self._mode = "current-limited":
			self.findRC()
			self.findR()
			self.findC()


	def findRC(self):
		self._RC = (2 * np.pi * self._filter_frequency)**-1


	def findR(self):
		if isinstance(self._RC, type(None) ):
			self.findRC()

		self._R = float(self._voltage) / float(self._current)

	def findC(self):
		self._C = self._RC / self._R




if __name__ == "__main__":
	filt = RCFilter(fc=2,voltage=1.8,current=2e-3)



