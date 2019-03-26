void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println(line_estimation());
}

float line_estimation() {
  const float x1 = 15, x2 = 5, x3 = -5, x4 = -15;
  int w1 = analogRead(A0), w2 = analogRead(A0), w3 = analogRead(A0), w4 = analogRead(A0); 
  int sum_of_weights = w1 + w2 + w3 + w4;
  int sum_weight_distance = x1*w1 + x2*w2 + x3*w3 + x4*w4;

  return sum_weight_distance/minimum(sum_of_weights, 1);
}

float minimum(float x, float y) {
  if (x <= y)
    return x;
  return y;
}
