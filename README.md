# PDF-Fuzz 설명
# sample.pdf는 메일에 같이 첨부하겠습니다.(저작권때문에요ㅠㅠ)
# Chapter 1 TinyInst - Runner
# 공식 문서에서는 아래와 같이 나와 있습니다. (TinyInst 기준)
mkdir build
cd build
cmake -G Xcode ..
cmake --build . --config Release
# 위와 같이 따라하시면 됩니다. "CMakeLists copy.txt"를 수정해둬서 위에 명령어 따라 치시면 "Release" 플더에 바이너리가 생성됩니다.]
# 방금 컴파일 한 친구가 Runner입니다.

# Chapter 2 PDF_Fuzz - PDF_Fuzz (PDF_Fuzz_Fix.cpp가 포함된 디렉토리로 이동)
clang++ -o PDF_Fuzz_Fix PDF_Fuzz_Fix.cpp -framework CoreGraphics -framework CoreFoundation

# Chapter 3 Fuzzer Run!
# 제가 보내드린 sample.pdf를 기준으로 Runner, PDF_Fuzz_Fix, sample.pdf가 같은 경로에 있다고 가정하고 말씀드리겠습니다.
mkdir crash
sudo ./Runner sample.pdf -- PDF_Fuzz_Fix
# -> sudo ./Runner 샘픔 -- 바이너리
# END!