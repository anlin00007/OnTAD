# OnTAD <a href="https://zenodo.org/badge/latestdoi/139454457"><img src="https://zenodo.org/badge/139454457.svg" alt="DOI"></a>
OnTAD is an Optimized Nested TAD caller for Hi-C data

##
## Getting Started

### Prerequisites
GNU Compiler Collection (https://gcc.gnu.org/) version >=5, with c++11 support


### Installing (Linux environment)
Here are two options to use OnTAD in local:

1. Users can download the executable file.

```
chmod +x OnTAD
```
Note: OnTAD is complied under gcc version 7.3.1. Please load same version to the environment, otherwise use the method below to complie the software.

2. OnTAD can be complied with source code in the /src folder. 

```
cd PathToOnTAD/src
make
chmod +x OnTAD
```
##Note: in some cases, static linking is neccessary. If it's needed, please add '-static' in line 3 of the makefile####

To remove OnTAD from your local folder:
```
make clean
```

3. Running and installing OnTAD through Docker (https://www.docker.com/)

The docker image of OnTAD can be found at https://cloud.docker.com/u/anlin00007/repository/docker/anlin00007/ontad

The image of current v1.2 version can be required by:
```
docker pull anlin00007/ontad:v1.2
```
To run OnTAD with Docker, one can simply convert the /docker/OnTAD to executable by:
```
chmod +x docker/OnTAD
```
Then run the following command to excute OnTAD:
```
bash OnTAD arg1 arg2 ....
```

Note: 

      1. user may need sudo access to run docker in some environment.

      2. running OnTAD with high resolution data may require large amount of memory. Please make sure you allocated enough memory for docker engine.
      
      3. advanced users may build images by their own with the docker/Dockerfile. Please check the image name/tag before you run docker/OnTAD.


### Examples
Call hierarchical TADs from the test matrix (http://bx.psu.edu/~lua137/OnTAD/chr18_KR.matrix).

The test matrix is from Rao et al, Cell 2014. And it is in Gm12878 with 10kb resolution.

```
OnTAD chr18_KR.matrix -penalty 0.1 -maxsz 200 -o OnTAD_KRnorm_pen0.1_max200_chr18
```
TAD calling results will be saved in OnTAD_KRnorm_pen0.1_max200_chr18.tad file.

New option in version 1.2 -- output TADs in bed format for genome browser virualization:

```
OnTAD chr18_KR.matrix -penalty 0.1 -maxsz 200 -o OnTAD_KRnorm_pen0.1_max200_chr18 -bedout 18 78077248 10000
```
A OnTAD_KRnorm_pen0.1_max200_chr18.bed file will be generated.

OnTAD result in Gm12878, chr18: 42.2Mb-44.8Mb (10Kb resolution)

<img src="https://github.com/anlin00007/OnTAD/blob/master/example_figure.png" width="400"/>

Now added new optional output: genome browser compatible bed file.
It provides an easy way for users to visualize their results.
Each tad region is marked by a specific color corresponding to the tad level:

<img src="https://github.com/anlin00007/OnTAD/blob/master/example_figure2.png" width="400"/>

An example of genome browser view in mouse G1E-ER4 chr19:

<img src="https://github.com/anlin00007/OnTAD/blob/master/example_figure3.png" width="800"/>

*The Hi-C heatmap view is from 3D genome browser.

### Results
The OnTAD output has five columns:

```
startpos  endpos  TADlevel  TADmean  TADscore
```

Explanations of each field are as follows:

  startpos & endpos: the begining and end of each TAD interval
  
  TADlevel: the level of each TAD in the hierarchy. Small value denotes outter TADs and large value denotes inner TADs.
  
  TADmean: average interaction frequency in each TAD region. (standardized by genomic distance)
  
  TADscore: confidence score for each TAD.
  
###Note: current version requires input data in the N * N matrix format and separated by TAB or space.



### Detailed Usage
```
OnTAD <Hi-C matrix> [-penalty <float>] [-maxsz <int>] [-minsz <int>] [-ldiff <float>] [-lsize <int>] [-bedout <chrnum int> <chrlength int> <int>] [-log2] [-o output_file]
```
\<Hi-C matrix\> the n*n Hi-C contact matrix. Both raw and normalized matrix are acceptable.

-penalty \<float\> The penalty applied in scoring function to select positive TADs. Higher penalty score will result in fewer TADs.

-maxsz \<int\> The maximum size of TADs can be called. The size is determined by number of bins covered in the contact matrix.

-minsz \<int\> The minimum size of TADs can be called. The size is determined by number of bins covered in the contact matrix.

-ldiff \<float\> The cut-off to determine local minimum. (local maximum - local minimum >= ldiff\*std)

-lsize \<int\> The local region size that used to determine local minimum

-log2 \<boolean\> if specified, log2(contact frequency) will be used to call TADs.

-bedout \<chrnum\> \<int\> The chromosome number, chromosome length and resolution (bp), e.g -bedout 3 198022430 10000 will generate bedfile with coordinates match chr3 at 10Kb resolution under reference genome hg19. ##Note: the function of chromosome length is to define the maximum position for the TAD at the end of input chromosome.

-o \<file path\> The file path for the TAD calling results.


  
### Defaults

-maxsz 200

-minsz 3

-penalty 0.1

-ldiff 1.96

-lsize 5

-bedout false


## Versioning
version 1.1; Added option to output genome browser compatible bed format for visualization

Current version 1.2; removed requirement of GSL package

## Reference
Lin An, Tao Yang, Jiahao Yang, Johannes Nuebler, Guanjue Xiang, Ross C. Hardison, Qunhua Li*, Yu Zhang*. OnTAD: Hierarchical Domain Structure Reveals the Divergence of Activity among TADs and Boundaries, Genome Biology 2019 (https://doi.org/10.1186/s13059-019-1893-y)

## License
This project is licensed under the MIT License - see the LICENSE.md file for details

## Acknowledgments
This work was supported by NIH R01 GM121613 and NIH R24 DK106766 (to YZ and LA), NIH training grant T32 GM102057 (CBIOS training program to The Pennsylvania State University), a Huck Graduate Research Innovation Grant to TY, and by NIH grants R01GM109453 (to QL).

## Contact
Lin An (lua137 {at} psu {dot} edu)
