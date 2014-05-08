#include <math.h>

#define ACD_start 60 //Value in MHz
#define ACD_stop 450 //Value in MHz
#define DAC_max 1023 //Decimal representation of register resolution

float dBMMinVals[391] = {2.11,2.15,2.23,2.26,2.31,2.37,2.46,2.51,2.56,2.67,2.75,2.82,2.92,3.04,3.11,3.28,3.3,3.448,3.633,3.79,3.99,4.12,4.33,5.0,5.27,5.61,5.75,5.96,6.12,6.4,6.7,7.02,7.53,7.84,8.26,8.76,9.38,10.21,8.35,9.2,9.36,9.38,9.42,9.41,9.31,8.63,12.72,12.51,12.48,12.59,12.81,13.12,13.0,12.96,13.12,13.42,13.83,14.19,14.57,14.67,14.71,14.65,14.659,14.465,14.05,13.32,12.87,12.19,11.54,11.04,10.49,10.1,9.73,9.31,9.09,8.87,8.48,8.23,8.03,7.37,6.21,6.6,6.39,6.118,5.75,5.36,5.01,4.59,4.23,3.84,3.34,3.05,2.67,2.32,1.54,2.48,0.98,0.27,0.0,-0.51,-0.74,-0.42,-0.32,-0.5,-0.6,-0.955,-0.104,-0.74,-1.1,-0.81,-0.86,-0.936,-0.87,-0.92,-0.943,-0.966,-1.01,-1.07,-1.03,-1.2,-1.28,-0.72,-0.83,-1.03,-1.33,-1.6,-1.91,-2.3,-2.82,-3.37,-2.81,-3.266,-2.61,-4.81,-6.3,-2.25,-3.69,-1.5,-0.3966,0.426,0.87,-1.48,-2.9,-3.3,-3.37,-3.48,-3.49,-3.5,-3.47,-3.59,-3.15,-3.56,-3.75,-3.6,-3.48,-3.59,-3.46,-3.58,-3.67,-3.82,-3.03,-2.52,-3.14,-3.35,-3.39,-3.3,-3.26,-3.55,-3.49,-3.44,-3.17,-3.33,-3.36,-3.62,-3.68,-3.59,-3.38,-3.26,-3.28,-3.57,-3.67,-3.63,-3.67,-3.66,-3.57,-3.45,-3.57,-3.97,-3.3,-3.1,-2.99,-2.89,-3.08,-3.04,-2.8,-2.4,-2.81,-2.96,-2.67,-2.77,-2.54,-2.44,-2.4,-2.45,-2.44,-2.3,-2.39,-2.42,-2.467,-2.0,-1.91,-1.918,-1.83,-1.87,-1.9,-1.65,-2.61,-1.27,-1.04,-2.21,-1.51,-1.39,-1.17,-0.98,-0.9,-0.46,-0.299,-0.11,-0.63,-2.93,0.3933,2.46,2.85,2.707,3.41,4.39,4.69,5.44,8.18,8.46,12.45,12.71,12.72,12.82,13.0559,13.229,13.3,13.398,13.43,13.46,13.45,13.43,13.41,13.38,13.35,13.31,13.12,13.08,13.04,13.01,7.59,6.94,6.25,5.77,5.24,4.76,4.37,4.12,3.98,3.01,2.163,1.58,1.611,1.13,1.03,1.07,0.81,0.43,0.04,-0.29,-0.98,-0.953,-0.91,-0.95,-0.37,-0.64,-2.1,-3.82,-2.58,-2.64,-3.06,-2.93,-3.0,-3.3,-3.64,-4.03,-3.83,-4.09,-4.12,-4.25,-4.53,-4.88,-5.27,-5.06,-5.8,-4.33,-5.33,-5.83,-6.21,-6.97,-6.262,-7.3,-6.87,-7.7,-7.81,-8.01,-8.82,-8.62,-6.89,-7.16,-7.1,-7.42,-7.59,-7.97,-8.57,-7.2,-10.553,-10.007,-9.33,-9.76,-9.67,-9.79,-10.13,-8.48,-7.64,-6.97,-7.44,-8.94,-8.91,-8.71,-8.022,-6.53,-8.03,-6.45,-5.36,-4.72,-3.63,-3.005,-2.025,-2.93,-1.87,-0.644,-1.66,-1.12,1.61,13.19,13.07,12.92,12.76,12.552,12.32,0.42,-1.69,-1.75,-1.97,-2.71,-3.81,-4.6,-5.0,-4.75,-4.53,-4.31,-4.26,-4.5,-6.41,-10.1,-13.88,-13.73,-8.17,-7.34,-6.78,-6.43,-6.2,-6.22,-18.388,-6.45,-6.391,-6.64,-6.8,-6.73,-4.9};

float dBMMaxVals[391] = {13.86,13.9,13.92,13.94,14.0,14.03,14.07,14.11,14.19,14.25,14.3,14.37,14.48,14.54,14.56,14.61,14.61,14.67,14.73,14.76,14.77,14.77,14.7,14.65,11.282,11.0,10.93,11.25,12.02,12.39,12.63,12.93,13.3,13.75,14.125,14.35,14.5,14.56,14.57,14.55,14.54,14.41,14.31,14.17,14.09,13.95,13.9,13.89,13.81,13.75,13.68,13.45,13.21,13.04,12.85,12.65,12.56,12.5,12.39,11.18,11.71,11.44,10.56,10.35,9.98,10.01,10.26,10.29,10.55,11.09,11.33,12.17,12.06,12.4,12.9,13.26,13.63,14.02,14.28,14.57,13.94,13.61,13.94,14.2,14.12,14.05,13.89,13.8,13.86,13.85,13.6,12.57,11.803,11.97,12.15,11.51,11.45,11.44,11.62,11.2,10.1,10.15,10.23,10.46,11.33,11.29,11.11,10.94,10.69,10.42,10.13,9.8,9.87,9.67,8.96,9.22,9.17,8.98,8.68,8.55,8.3,8.35,7.82,7.37,6.1,6.31,6.3,6.28,6.24,6.29,6.4,6.44,6.5,6.5,7.49,7.69,7.02,7.5,7.78,7.84,7.48,7.44,7.4,7.35,6.97,7.1,7.12,7.44,7.82,6.85,7.15,7.0,7.06,7.05,7.11,6.94,7.1,6.84,7.05,6.87,7.1,7.23,7.18,7.28,7.49,7.43,7.47,7.19,7.14,7.17,7.057,7.37,7.62,7.83,7.79,7.88,7.62,7.8,7.56,7.85,7.79,7.7,7.95,7.57,7.79,7.97,7.71,7.67,7.7,7.78,8.382,8.28,8.16,8.3,8.39,8.61,8.54,8.84,8.86,9.03,9.21,9.41,9.5,9.68,9.77,9.98,10.1,10.07,10.02,10.34,10.44,10.57,10.68,10.73,10.99,11.14,11.38,11.55,11.8,11.26,11.29,12.47,12.95,13.13,12.85,13.25,13.475,13.27,13.16,12.98,12.99,12.84,12.53,11.66,11.56,11.43,11.53,11.66,11.8,11.72,11.8,11.72,11.71,11.73,11.72,11.61,11.54,11.54,11.45,11.45,11.42,11.38,11.35,11.38,11.39,11.48,11.446,11.56,11.75,11.69,11.86,11.95,12.05,12.23,12.3,12.34,12.35,12.42,12.54,12.6,12.71,12.76,12.91,13.03,12.92,12.88,12.57,12.52,12.13,11.51,11.22,10.95,10.3,9.75,8.7,7.95,6.556,4.58,8.51,8.84,9.11,9.08,8.54,8.38,8.36,8.7,8.61,8.23,8.38,7.97,6.86,5.6,0.7763,8.0,5.682,4.65,2.48,1.95,1.322,0.73,0.35,-1.34,-1.13,0.0,-0.41,0.16,1.3,1.7019,1.2,1.15,1.5,1.65,1.92,2.25,2.12,2.78,3.08,3.23,2.79,-1.68,-2.11,-2.01,-1.67,-2.1,-2.14,-1.945,-1.3,-1.0,-0.9,7.79,7.98,7.83,8.0,8.11,8.37,8.65,9.1,9.69,10.33,12.781,12.618,12.423,12.32,12.32,12.411,12.46,12.52,12.57,12.57,12.52,12.42,12.28,12.199,12.18,12.23,12.32,12.37,12.39,12.36,12.31,12.21,11.98,11.71,10.67,8.73,6.66,2.3,-0.76,-6.6,-11.5,-11.41,-9.76,-8.6,-8.5,-8.26,-9.98,-8.756,-6.87,-6.887,-6.97,-6.87};

float getDBM(uint32_t frequency, uint16_t DAC_val) {
  uint32_t fetchPos;
  float weight;
  float a,b,c,offset;
  float minW,maxW;
  if(frequency >= ACD_start * pow(10,6) && frequency < ACD_stop * pow(10, 6)) {
    uint32_t remainder = frequency % 1000000;
    fetchPos = (frequency - remainder) * pow(10 , -6);
    weight = remainder * pow(10 , -6);
    minW = (1 - weight) * dBMMinVals[fetchPos - ACD_start] + weight * dBMMinVals[fetchPos + 1 - ACD_start];
    maxW = (1 - weight) * dBMMaxVals[fetchPos - ACD_start] + weight * dBMMaxVals[fetchPos + 1 - ACD_start];
  }
  else if (frequency == (uint32_t) (ACD_stop * pow(10,6))) {
    minW = dBMMinVals[ACD_stop - ACD_start];
    maxW = dBMMaxVals[ACD_stop - ACD_start];
  } else {
    return -1;
  }
  if(minW > 0 && maxW > 0) {
    offset = 0;
  } else {
    offset = (minW < maxW) ? (1 - minW) : (1 - maxW);
  }
  if(offset > 0) {
    maxW += offset;
    minW += offset;
  }
  b = pow((maxW / minW) , (1.0 / DAC_max));
  a = maxW / pow(b , DAC_max);
  if(offset == 0)
    return a * pow(b , DAC_val);
  else
    return a * pow(b , DAC_val) -(offset - 1) - a;
}

float getVpp(uint32_t frequency, uint16_t DAC_val) {
  float dBM = getDBM(frequency, DAC_val);
  float pow_val = 0.1*(dBM - 30);
  float sqrt_res = 50 * pow(10 , pow_val);
  if(sqrt_res > 0) {
    return 2*sqrt(2)*sqrt(sqrt_res);
  }
  else 
    return 0;
}

uint16_t findBestDACMatch(uint32_t freq, float searchVal) {
  float diff = abs(searchVal - getDBM(freq , 0));
  uint16_t DAC = 0;
  float currDBM;
  for(uint16_t i = 0; i <= 1023; i++) {
    currDBM = getDBM(freq , i);
    if(fabs(searchVal - currDBM) < diff) {
      diff = fabs(searchVal - currDBM);
      DAC = i;
    }
  }
  return DAC;
}

uint32_t findIncFreqDBMMatch(uint32_t frequency, float search_val, uint32_t step_size) {
  for(uint32_t searchFreq = frequency + step_size; searchFreq <= ACD_stop * pow(10 , 6); searchFreq += step_size) {
    if(getDBM(searchFreq, 0) <= search_val)
      return searchFreq;
  }
  return frequency;
}

uint32_t findDecFreqDBMMatch(uint32_t frequency, float search_val, uint32_t step_size) {
  for(uint32_t searchFreq = frequency - step_size; searchFreq >= ACD_start * pow(10 , 6); searchFreq -= step_size) {
    if(getDBM(searchFreq, 0) <= search_val)
      return searchFreq;
  }
  return frequency;
}