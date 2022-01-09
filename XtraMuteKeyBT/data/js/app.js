$(document).ready(function() {
    $("#status").html("Connecting, please wait...");
    $.get("config.json", function(data) {
        $("#status").html("Loading config, please wait...");
        parseData(data);
       
    }).fail(function() {
        $.get("config-dev.json", function(data) {
            $("#status").html("Loading Dev config, please wait...");
            parseData(data);
           
        })
    });
  
});

function parseData(data)
{
    $("#status").html("Loaded");
    console.log(data.currentOptions);
    console.log(data.settings.numButtons);
    for (let i = 0; i < data.settings.numButtons; i++) {
        addButton(i,data.options,data.currentOptions[i])
        
    }
    $.each(data.options, function(index, option) {
        console.log(option);
      })
}


function addButton(btnNr, options, selectedIndex) {
    var template = $("#buttonTemplate").clone();
  
    template.attr("id", "form-group-" + btnNr);
  
    var id = "input-btn-" + btnNr;
  
    var label = template.find(".control-label");
    label.attr("for", id);
    label.text("Button "+(btnNr+1));
  
    var select = template.find(".form-control");
    select.attr("id", id);
  
    for (var i = 0; i < options.length; i++) {
      var optionText = options[i].description;
      var option = $("<option></option>");
      option.text(optionText);
      option.attr("value", options[i].index);
      select.append(option);
    }
  
    select.val(selectedIndex);
  
    select.change(function() {
        var value = template.find("#" + id + " option:selected").index();
      console.log(btnNr, value);
    //   postValue(field.name, value);
    //   if (field.name == "pattern") {
    //     updateDisplayedFields(value);
    //   }
    });
  
    
  
    $("#form").append(template);
  }