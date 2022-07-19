int flame = A0;                    //포토트랜지스터가 연결된 아날로그핀
int Buzzer_Pin = A1;
int val = 0;                       //센서출력값 저장 변수

void setup() {
//  pinMode(flame,INPUT);                 //포토트랜지스터 입력설정
  pinMode(Buzzer_Pin, OUTPUT);
  Serial.begin(115200);                  //시리얼모니터 설정
}
 
void loop() {
  val = analogRead(flame);                         //포토트랜지스터에서 값을 읽어옴
  Serial.println(val);                     //포토트랜지스터 입력값 시리얼모니터로 출력
  if(val < 50){
    digitalWrite(5, LOW);
    analogWrite(6, 0);
  }
    else{
      digitalWrite(5, HIGH);
      analogWrite(6, 255);
    }
  delay(1000);
}