



struct NetstreamParameters{
	int handle;
	bool reading_netstream = false;
	void *header_buffer;
	int error_status;
	unsigned int buffer_size;
}	


int readOffNetStream(uint16_t port){
	//opening and checking the netstream
	stream_ok = enI106OpenNetStream(NetstreamParameters.handle,port);
	while (stream_ok == I106_OK){
		NetstreamParameters.reading_netstream = true;
		
		//reading the next header off of netstream
		NetstreamParameters.error_status = en106_ReadNetStream(NetstreamParameters.handle,&header_buffer,
	}
	
	
}
