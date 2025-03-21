# Upgrade plan for Easter  

## Standardize light bar ends  
I think it would be a good idea to choose a standardized connector for the ends of the lights. 
Then we can design a modular command block that plugs right into the end of the fixture. 
By using max485 converters at each fixture, the controllers can be located much farther away. 
[Max485 for transmitting WS LED signals. ](https://blinkylights.blog/2020/12/08/transmitting-ws2812b-signals-over-longer-distances/)

## Power cable
In order to move the power supplies farther from the fixtures, we need suitable power cable and connectors. 
XT30 or XT60 connectors should work well for this. 
Data lines also need to be run to the fixtures. I think instead of getting an all in one cable, we should just used twisted pair and parallel pair for data and power respectively. 
We can run the entire thing inside of a 
[black wire loom](https://www.aliexpress.us/item/3256807384802224.html?spm=a2g0o.productlist.main.39.71d13bf024Fjn5&aem_p4p_detail=202503211015036400609243362880000669794&algo_pvid=0c0606d9-d4e9-4fb8-86a4-641d34033291&algo_exp_id=0c0606d9-d4e9-4fb8-86a4-641d34033291-19&pdp_ext_f=%7B%22order%22%3A%221356%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%211.15%210.96%21%21%211.15%210.96%21%40210318c317425773034056717e8773%2112000041340380153%21sea%21US%216085429096%21X&curPageLogUid=C6K4L1dIpSGF&utparam-url=scene%3Asearch%7Cquery_from%3A&search_p4p_id=202503211015036400609243362880000669794_6) 
to hide the colors. 
If there is issues with voltage sag, we may need to switch to a 12 volt lighting system. 
