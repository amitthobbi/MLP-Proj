x is a 8 bit value.
256 entries

int sigmoid(int8_t x)
{
    //assume this is global
    //int logistic_lut[index_t];

     // Convert input x to index_t
     // 8/256 ~= 0.03137 <-> 8/0.03137 ~= 255
     // x=8 will return 255, 0 will return 0
	int index_t = x / 0.03137;

    //Return output as per the input
	if (x > 8)
	{
		return 1;
	}
	else if (x <= 8 && x > 0)
	{
        return logistic_lut[index_t];
	}
	else if (x <= 0 && x >=-8)
	{
		return (1 - logistic_lut[index_t]);
	}
	else if (x < -8 )
	{
		return 0;
	}
	else
	{
	 //Should not go here
	 cout <<"error Invalid value!!";
	 return -1;
	}
}