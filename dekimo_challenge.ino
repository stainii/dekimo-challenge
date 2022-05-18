unsigned long time;
String minimalTrailingZeroes = "000000000000000000000000";
int numbersToIgnoreInRange[43] = {29, 30, 36, 42, 48, 54, 60, 61, 67, 73, 79, 85, 91, 92, 98, 104, 110, 116, 122, 123, 129, 135, 141, 147, 153, 154, 155, 161, 167, 173, 179, 185, 186, 192, 198, 204, 210, 216, 217, 223, 229, 235, 241};

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  String number = Serial.readStringUntil('\n');

  if (number.length() == 0) {
    return;
  }
  
  time = micros(); 
  digitalWrite(2, HIGH);

  int factorial = determineFactorial(number);
  Serial.print(factorial);
  Serial.print("!");
  
  printMissingDigits(number);

  time = micros() - time;
  digitalWrite(2, LOW);
  
  Serial.println(time * 1000);
}

int determineFactorial(String number) {
  long lengthNumber = number.length();
  int startOfTrailingZeroes = number.indexOf(minimalTrailingZeroes);
  int numberOfTrailingZeroes = lengthNumber - startOfTrailingZeroes;
  int possibilities[5] = {0,0,0,0,0};
  determinePossibilitiesBasedOnTrailingZeroes(numberOfTrailingZeroes, possibilities);

  for (int i = 0; i < 4; i++) {
    if (hasSameLength(possibilities[i], number)) {
      return possibilities[i];
    }
  }
  return possibilities[4];
}

void determinePossibilitiesBasedOnTrailingZeroes(int numberOfTrailingZeroes, int (& possibilities) [5]) {
  int skip = 0;

  for (int i = 0; i < 43; i++) { // todo de lengte kan ik hardcoderen
    if (numberOfTrailingZeroes < numbersToIgnoreInRange[i]) {
      skip = i;
      break;
    }
    if (i == 42) {
      skip = 43;
    }
  }

  int index = 5 * (numberOfTrailingZeroes - 24 - skip);
  possibilities[0] = 100 + index;
  possibilities[1] = 101 + index;
  possibilities[2] = 102 + index;
  possibilities[3] = 103 + index;
  possibilities[4] = 104 + index;
}

boolean hasSameLength(int n, String number) {
  double x = (n * log10(n / 2.7182) + log10(2 * 3.141592 * n) / 2.0) / log10(10);
  int length = (int) floor(x);
  return length == number.length() || length + 1 == number.length();
}

String printMissingDigits(String number) {
  int positionOfX = number.indexOf("x");
  String partBeforeX = number.substring(0, positionOfX);
  String partAfterY = number.substring(positionOfX + 2);

  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 10; y++) {
      if (x != 0 || y != 0 ) {
        if (isDivisibleBy9And11(partBeforeX + x + y + partAfterY)) {
          Serial.print("xy=");
          Serial.print(x);
          Serial.print(",");
          Serial.println(y);
        }
      }
    }  
  }
  return "FOUT";
}

boolean isDivisibleBy9And11(String number) {
    int sumAllDigits = 0;
    int sumEvenDigits = 0;
    int sumOddDigits = 0;

    for (int i = 0; i < number.length(); i++) {
        int digit = number.charAt(i) - '0';
        sumAllDigits += digit;

        if (i % 2 == 0) {
            sumEvenDigits += digit;
        } else {
            sumOddDigits += digit;
        }
    }
    return sumAllDigits % 9 == 0 && (sumOddDigits - sumEvenDigits) % 11 == 0;
}
