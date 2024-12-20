/*
 * ArrayUtil.h
 *
 *  Created on: 13 gru 2024
 *      Author: kosarzewski.1
 */

#ifndef ARRAYUTIL_H_
#define ARRAYUTIL_H_


void printArray(double *arr, int size);


void printArray(double *arr, int size)
{


	for (int i = 0; i < size; ++i) {

		cout<<arr[i]<<", ";

	}

	cout<<endl;

}


#endif /* YUTIL_H_ */
