(really) ad-hoc program used to check the Alice DQM MCH shifter agent, but offline, i.e. without the 
burden of Amore stuff.

Compilation requires Root, AliRoot and AliEn, e.g. something along the lines of :

```
cmake $HOME/alice/ali-master/mchdqm \
-DALIROOT=$HOME/alice/sw/osx_x86-64/AliRoot/latest-ali-master-release \
-DROOTSYS=$HOME/alice/sw/osx_x86-64/ROOT/v5-34-30-alice7-1 \
-DALIEN=$HOME/alice/sw/osx_x86-64/AliEn-Runtime/latest
```

(assuming a typical alibuild installation)

Example usage : 

```
mchqashifter --ocdb local:///alice/data/2016/OCDB --input /alice/data/2016/LHC16j/000256289/raw/16000256289037.8802.root
```
