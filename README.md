# PMLSH: A Fast and Accurate LSH Framework for High Dimensional Approximate NN Search
-----------------------------------------------------------------------------------------------------------------
## Introduction
This is a source code for the algorithm described in the paper **PMLSH: A Fast and Accurate LSH Framework for High Dimensional Approximate NN Search (PVLDB 2020, one of the best paper)**. We call it as **PM-LSH** project.

## Compilation

**PM-LSH** project is written by **C++** and is simple and easy to use. It can be complied by **g++** in **Linux** and **MSVC** in **Windows**.

### Dependency

In order to compile the program, you should have the dependencies of **Boost**. For example, for Ubuntu users, you can install Boost with the following commands:
```bash
sudo apt-get install libboost-dev
sudo apt-get install libboost-program-options-dev
```
For Windows users, you can download Boost library and put it in the include path.


### Installation
#### Windows
We can use **Visual Studio 2019** (Other version of Visual Studio should also work but remains untested) to build the project with importing all the files in the directory `./pmlsh/src/`.

#### Linux
```bash
cd ./pmLSH
make
```
The excutable file is then in dbLSH directory, called as `pmlsh`

## Usage

### Command Usage

-------------------------------------------------------------------
> pmlsh datasetName n dim m c beta R_min
-------------------------------------------------------------------
(the first parameter specifies the procedure be executed and change)

### Parameter explanation

- datasetName  : dataset name
- n            : a positive integer, the cardinality of dataset
- dim          : a positive integer, the dimensionality of dataset
- c            : 0-1, a float number, the approximate ratio
- beta         : 0-1. a float number, the maximum ratio of the number of returned points to the total number of dataset   
- R_min        : a float number, the inital radius
-------------------------------------------------------------------

FOR EXAMPLE, YOU CAN RUN THE FOLLOWING CODE IN COMMAND LINE AFTER BUILD ALL THE TOOLS:

```bash
cd ./pmLSH
./pmlsh audio 54187 192 15 1.5 0.06 1.0
```

## Dataset

In our project, the format of the input file (such as `audio.data_all, audio.data_all_query`, which is in `float` data type) is the fecvs format. It is a binary file but not a text file, because binary file has many advantages. The binary file is organized as the following format:

>{indice: i} {The binary vector of data[i]}(arranged in turn)

FOR EXAMPLE, if a potential file called `test.data_all` presents a dataset `test` with `n=3,dim=2`, it can be organized as follow:
```bash
0 0.1 0.2
1 -0.3 1.5
2 0.2 0.4
```

For your application, you should also transform your dataset into this binary format, then rename it as `[datasetName].data_all`(dataset) and `[datasetName].data_all_query`(query set) and put it in the directory `./dataset`.

A sample dataset `audio.data_all` has been put in the directory `./dataset`.

For other dataset we use, you can get the raw data from following links: [MNIST](http://yann.lecun.com/exdb/mnist/index.html), [Cifar](http://www.cs.toronto.edu/~kriz/cifar.html), [Trevi](http://phototour.cs.washington.edu/patches/default.htm), [NUS](https://pan.baidu.com/share/init?surl=kVKfXFx)(Extraction code: hpxg), [Deep1M](https://www.cse.cuhk.edu.hk/systems/hash/gqr/dataset/deep1M.tar.gz), [GIST](http://corpus-texmex.irisa.fr/), [TinyImages80M](https://hyper.ai/tracker/download?torrent=6552), [SIFT](http://corpus-texmex.irisa.fr/). Next, you should transform your raw dataset into the mentioned binary format, then rename it as `[datasetName].data_all`(dataset) and `[datasetName].data_all_query`(query set) and put it in the directory `./dataset`.


## Result
The experimental result is saved in the directory `./pmlsh/experiment/` as the file
`result.csv` and `result_all.csv`


## Acknowledgement
**PM-LSH** project is developed by referring to [LSHBOX](https://github.com/RSIA-LIESMARS-WHU/LSHBOX). Great appreciation to the contributors of LSHBOX.

## Reference
**[Bolong Zheng, Xi Zhao, Lianggui Weng, Nguyen Quoc Viet Hung, Hang Liu, Christian S. Jensen: PM-LSH: A Fast and Accurate LSH Framework for High-Dimensional Approximate NN Search. Proc. VLDB Endow. 13(5): 643-655 (2020) (One of the best PVLDB 2020 papers)](http://www.vldb.org/pvldb/vol13/p643-zheng.pdf)**

Please use the following bibtex to cite this work when you use **PM-LSH** in your paper.

```tex
@article{DBLP:journals/pvldb/ZhengZWHLJ20,
	author    = {Bolong Zheng and
	Xi Zhao and
	Lianggui Weng and
	Nguyen Quoc Viet Hung and
	Hang Liu and
	Christian S. Jensen},
	title     = {{PM-LSH:} {A} Fast and Accurate {LSH} Framework for High-Dimensional
	Approximate {NN} Search},
	journal   = {Proc. {VLDB} Endow.},
	volume    = {13},
	number    = {5},
	pages     = {643--655},
	year      = {2020}
}
```

If you meet any issue on the code or take interest in our work, please feel free to contact me (xizhao@ust.hk) or Prof. Zheng (zblchris@gmail.com). Thank you.
