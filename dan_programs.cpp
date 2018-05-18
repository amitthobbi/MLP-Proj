#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define JAM 5000
#define SAT 5000
#define MIN_INT16 100
#define MAX_INT16 100000

int Mul16( int ,int  );
int Extract16( int , int , int ,int );
void Sigmoid( struct node *, int );

int Mul16( int op1,int  op2)
{

int product;

product = op1 * op2;

return(product);

}

int Extract16( int op, int pos, int jam,int sat)
{
int tmp, lower, upper;

if (pos >=0) tmp = op >> pos;

else tmp = op << -pos;  /* if pos is negative */

upper = tmp >> 16;

lower = FIELD(op, pos, 0);

/* jammin' ? */

if (jam == JAM) {
if (lower !=0) tmp |=1;

}

/* check for saturation */

if (sat == SAT) {
/* If underflow, set tmp to the minimum value. */
if (tmp < MIN_INT16) {
tmp = MIN_INT16;

}
else

/* If overflow, set tmp to the maximum value. */

if (tmp > MAX_INT16) {
tmp = MAX_INT16;

}

}

return(tmp);
}


void Sigmoid( struct node *I, int n )
{

int i, val, idx, sum, tmp, inter, tmpsum;

/* for all nodes in I */

for(i=0; i<n; i++) {

tmpsum = Extract16( (I+i)->sum, 7, JAM, SAT);
/* s13.18 -> s3.12 */

idx = ( tmpsum >> 8 ) & 0xFF;

val = sigmoid_tab[idx];
if (idx == 0xFF) {

/* largest neg #, for interpolation use */
inter = 0xFFFFFFFF;}

else if (idx == 0x7F) {

/* largest pos #, for interpolation use */
inter = 0x7FE9;}

else {

inter = sigmoid_tab[idx+1];}

/* now do interpolation */

tmp = inter - val;

sum = tmp * (tmpsum & 0xFF);
sum = Extract16( sum, 8, JAM, SAT);

(I+i)->out = sum + val;  /* s.15 */

}

}


