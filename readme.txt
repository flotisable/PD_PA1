fileencoding: utf8

學號:         r06943085
姓名:         黃韋智
程式語言:     C++
編譯器:       g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
檔案壓縮方式: tar -czf r06943085-p1.tar.gz r06943085-p1
各檔案說明:

  r0943085-p1
  |
  |--- src
  |    |
  |    |--- main.cpp:       主程式原始碼
  |    |--- parser.h:       輸入檔剖析器原始碼
  |    |--- parser.cpp:     輸入檔剖析器原始碼
  |    |--- FMPartiter.h:   FM 演算法原始碼
  |    |--- FMPartiter.cpp: FM 演算法原始碼
  |    |--- data.h:         Cell 與 Net 資料結構原始碼
  |    |--- makefile:       編譯程式的 makefile
  |    |--- fm:             主程式
  |
  |--- readme.txt:  本文件
  |--- report.doc:  說明所使用的資料結構的文件 

編譯方式說明:

  在 r0943085-p1 目錄下輸入 make release 即可完成編譯
  編譯完成後會產生名為 fm 的執行檔

執行方式:

  fm <input_file> <output_file>
