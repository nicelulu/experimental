OS: Ubuntu 20.04.4 LTS  
Compiler: Clang 13.0.1  
CPU: Intel(R) Xeon(R) Gold 6267C CPU @ 2.60GHz  
CMAKE_BUILD_TYPE: Release  

```
./test1 10000000 1 32
testAVX Time: 			        0.014388 sec res_size: 10000000
testSSE Time: 				0.015849 sec res_size: 10000000
testDirectly Time: 			0.015540 sec res_size: 10000000
filterAVX Time: 			0.017517 sec res_size: 10000000
filterSSE Time: 			0.021554 sec res_size: 10000000
filterMethod Time: 			0.024100 sec res_size: 10000000
genFilterSSE filterAVX Time: 		0.020580 sec res_size: 10000000
genFilterSSE filterSSE Time: 		0.027200 sec res_size: 10000000
genFilterSSE filterMethod Time: 	0.026544 sec res_size: 10000000

./test1 10000000 2 32
testAVX Time: 				0.061533 sec res_size: 5001755
testSSE Time: 				0.064234 sec res_size: 5001755
testDirectly Time: 			0.069574 sec res_size: 5001755
filterAVX Time: 			0.049632 sec res_size: 5001755
filterSSE Time: 			0.049326 sec res_size: 5001755
filterMethod Time: 			0.060305 sec res_size: 5001755
genFilterSSE filterAVX Time: 		0.081591 sec res_size: 5001755
genFilterSSE filterSSE Time: 		0.081586 sec res_size: 5001755
genFilterSSE filterMethod Time: 	0.090706 sec res_size: 5001755

./test1 10000000 4 32
testAVX Time: 				0.051762 sec res_size: 2501728
testSSE Time: 				0.053866 sec res_size: 2501728
testDirectly Time: 			0.060157 sec res_size: 2501728
filterAVX Time: 			0.037587 sec res_size: 2501728
filterSSE Time: 			0.036783 sec res_size: 2501728
filterMethod Time: 			0.044125 sec res_size: 2501728
genFilterSSE filterAVX Time: 		0.067632 sec res_size: 2501728
genFilterSSE filterSSE Time: 		0.067434 sec res_size: 2501728
genFilterSSE filterMethod Time: 	0.077246 sec res_size: 2501728

./test1 10000000 6 32
testAVX Time: 				0.044591 sec res_size: 1668032
testSSE Time: 				0.042682 sec res_size: 1668032
testDirectly Time: 			0.035110 sec res_size: 1668032
filterAVX Time: 			0.030266 sec res_size: 1668032
filterSSE Time: 			0.030040 sec res_size: 1668032
filterMethod Time: 			0.035924 sec res_size: 1668032
genFilterSSE filterAVX Time: 		0.052145 sec res_size: 1668032
genFilterSSE filterSSE Time: 		0.051808 sec res_size: 1668032
genFilterSSE filterMethod Time: 	0.061287 sec res_size: 1668032

./test1 10000000 8 32
testAVX Time: 				0.036265 sec res_size: 1249442
testSSE Time: 				0.034455 sec res_size: 1249442
testDirectly Time: 			0.033748 sec res_size: 1249442
filterAVX Time: 			0.026556 sec res_size: 1249442
filterSSE Time: 			0.026669 sec res_size: 1249442
filterMethod Time: 			0.031512 sec res_size: 1249442
genFilterSSE filterAVX Time: 		0.043028 sec res_size: 1249442
genFilterSSE filterSSE Time: 		0.042838 sec res_size: 1249442
genFilterSSE filterMethod Time: 	0.051190 sec res_size: 1249442

./test1 10000000 10 32
testAVX Time: 				0.031282 sec res_size: 1001129
testSSE Time: 				0.030031 sec res_size: 1001129
testDirectly Time: 			0.054498 sec res_size: 1001129
filterAVX Time: 			0.024905 sec res_size: 1001129
filterSSE Time: 			0.024525 sec res_size: 1001129
filterMethod Time: 			0.030032 sec res_size: 1001129
genFilterSSE filterAVX Time: 		0.038365 sec res_size: 1001129
genFilterSSE filterSSE Time: 		0.038695 sec res_size: 1001129
genFilterSSE filterMethod Time: 	0.046284 sec res_size: 1001129

./test1 10000000 12 32
testAVX Time: 				0.028356 sec res_size: 830910
testSSE Time: 				0.027628 sec res_size: 830910
testDirectly Time: 			0.026778 sec res_size: 830910
filterAVX Time: 			0.024433 sec res_size: 830910
filterSSE Time: 			0.024184 sec res_size: 830910
filterMethod Time: 			0.029718 sec res_size: 830910
genFilterSSE filterAVX Time: 		0.036321 sec res_size: 830910
genFilterSSE filterSSE Time: 		0.036334 sec res_size: 830910
genFilterSSE filterMethod Time: 	0.042871 sec res_size: 830910

./test1 10000000 14 32
testAVX Time: 				0.025660 sec res_size: 714642
testSSE Time: 				0.025519 sec res_size: 714642
testDirectly Time: 			0.025603 sec res_size: 714642
filterAVX Time: 			0.023757 sec res_size: 714642
filterSSE Time: 			0.023515 sec res_size: 714642
filterMethod Time: 			0.029512 sec res_size: 714642
genFilterSSE filterAVX Time: 		0.034419 sec res_size: 714642
genFilterSSE filterSSE Time: 		0.034565 sec res_size: 714642
genFilterSSE filterMethod Time: 	0.040942 sec res_size: 714642

./test1 10000000 16 32
testAVX Time: 				0.023535 sec res_size: 625625
testSSE Time: 				0.023819 sec res_size: 625625
testDirectly Time: 			0.024710 sec res_size: 625625
filterAVX Time: 			0.023167 sec res_size: 625625
filterSSE Time: 			0.022844 sec res_size: 625625
filterMethod Time: 			0.029538 sec res_size: 625625
genFilterSSE filterAVX Time: 		0.032938 sec res_size: 625625
genFilterSSE filterSSE Time: 		0.032754 sec res_size: 625625
genFilterSSE filterMethod Time: 	0.039963 sec res_size: 625625

./test1 10000000 18 32
testAVX Time: 				0.022050 sec res_size: 555776
testSSE Time: 				0.022671 sec res_size: 555776
testDirectly Time: 			0.023980 sec res_size: 555776
filterAVX Time: 			0.023143 sec res_size: 555776
filterSSE Time: 			0.022564 sec res_size: 555776
filterMethod Time: 			0.029771 sec res_size: 555776
genFilterSSE filterAVX Time: 		0.031962 sec res_size: 555776
genFilterSSE filterSSE Time: 		0.031632 sec res_size: 555776
genFilterSSE filterMethod Time: 	0.039412 sec res_size: 555776

./test1 10000000 20 32
testAVX Time: 				0.020929 sec res_size: 499933
testSSE Time: 				0.021653 sec res_size: 499933
testDirectly Time: 			0.023541 sec res_size: 499933
filterAVX Time: 			0.022716 sec res_size: 499933
filterSSE Time: 			0.022286 sec res_size: 499933
filterMethod Time: 			0.029969 sec res_size: 499933
genFilterSSE filterAVX Time: 		0.030937 sec res_size: 499933
genFilterSSE filterSSE Time: 		0.030379 sec res_size: 499933
genFilterSSE filterMethod Time: 	0.039100 sec res_size: 499933

./test1 10000000 22 32
testAVX Time: 				0.019941 sec res_size: 453672
testSSE Time: 				0.021038 sec res_size: 453672
testDirectly Time: 			0.023037 sec res_size: 453672
filterAVX Time: 			0.022337 sec res_size: 453672
filterSSE Time: 			0.021621 sec res_size: 453672
filterMethod Time: 			0.029785 sec res_size: 453672
genFilterSSE filterAVX Time: 		0.030220 sec res_size: 453672
genFilterSSE filterSSE Time: 		0.029346 sec res_size: 453672
genFilterSSE filterMethod Time: 	0.039016 sec res_size: 453672

./test1 10000000 24 32
testAVX Time: 				0.019210 sec res_size: 415811
testSSE Time: 				0.020589 sec res_size: 415811
testDirectly Time: 			0.022728 sec res_size: 415811
filterAVX Time: 			0.022121 sec res_size: 415811
filterSSE Time: 			0.021200 sec res_size: 415811
filterMethod Time: 			0.030543 sec res_size: 415811
genFilterSSE filterAVX Time: 		0.029357 sec res_size: 415811
genFilterSSE filterSSE Time: 		0.028589 sec res_size: 415811
genFilterSSE filterMethod Time: 	0.038607 sec res_size: 415811

./test1 10000000 26 32
testAVX Time: 				0.018507 sec res_size: 384378
testSSE Time: 				0.019679 sec res_size: 384378
testDirectly Time: 			0.022467 sec res_size: 384378
filterAVX Time: 			0.021892 sec res_size: 384378
filterSSE Time: 			0.020633 sec res_size: 384378
filterMethod Time: 			0.029866 sec res_size: 384378
genFilterSSE filterAVX Time: 		0.028764 sec res_size: 384378
genFilterSSE filterSSE Time: 		0.027879 sec res_size: 384378
genFilterSSE filterMethod Time: 	0.038026 sec res_size: 384378

./test1 10000000 28 32
testAVX Time:				0.018181 sec res_size: 356824
testSSE Time: 				0.019381 sec res_size: 356824
testDirectly Time: 			0.024022 sec res_size: 356824
filterAVX Time: 			0.021695 sec res_size: 356824
filterSSE Time: 			0.020242 sec res_size: 356824
filterMethod Time: 			0.029832 sec res_size: 356824
genFilterSSE filterAVX Time: 		0.028411 sec res_size: 356824
genFilterSSE filterSSE Time: 		0.027116 sec res_size: 356824
genFilterSSE filterMethod Time: 	0.037654 sec res_size: 356824

./test1 10000000 30 32
testAVX Time: 				0.017520 sec res_size: 333398
testSSE Time: 				0.018915 sec res_size: 333398
testDirectly Time: 			0.023713 sec res_size: 333398
filterAVX Time: 			0.021058 sec res_size: 333398
filterSSE Time: 			0.019511 sec res_size: 333398
filterMethod Time: 			0.029255 sec res_size: 333398
genFilterSSE filterAVX Time: 		0.027370 sec res_size: 333398
genFilterSSE filterSSE Time: 		0.026196 sec res_size: 333398
genFilterSSE filterMethod Time: 	0.037156 sec res_size: 333398
```
