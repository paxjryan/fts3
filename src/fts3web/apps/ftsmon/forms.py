from django import forms
from django.core.exceptions import ValidationError
import json


class JobSearchForm(forms.Form):
    jobId = forms.CharField(required = False)
    

    
class StateField(forms.CharField):
    def to_python(self, value):
        if not value:
            return None
        states = []
        for s in value.split(','):
            s = s.strip()
            if s:
                states.append(s)
        return states
    
    def prepare_value(self, value):
        return self.to_python(value)
    
    def clean(self, value):
        self.validate(value)
        return self.to_python(value)

    def bound_data(self, data, initial):
        py = self.to_python(data)
        if py:
            return ', '.join(py)
        else:
            return None

    def validate(self, value):
        states = self.to_python(value)
        if states is not None:
            for s in states:
                if s not in ['SUBMITTED', 'READY', 'ACTIVE', 'FINISHED', 'FINISHEDDIRTY', 'FAILED', 'CANCELED']:
                    raise ValidationError("'%s' is not a valid state" % s)


class JsonField(forms.CharField):
    def to_python(self, value):
        if not value:
            return None
        return json.loads(value)
    
    def prepare_value(self, value):
        return self.to_python(value)
    
    def clean(self, value):
        self.validate(value)
        return self.to_python(value)
    
    def bound_data(self, data, initial):
        return data
    
    def validate(self, value):
        try:
            if value: json.loads(str(value))
        except Exception, e:
            raise ValidationError('Invalid JSON: ' + str(e))
    

class FilterForm(forms.Form):
    source_se   = forms.CharField(required = False)
    dest_se     = forms.CharField(required = False)
    state       = StateField(required = False)
    vo          = forms.CharField(required = False)
    metadata    = JsonField(required = False)
    time_window = forms.IntegerField(required = False) 
    
    def is_empty(self):
        if self['source_se'].data or self['dest_se'].data or\
           self['state'].data or self['vo'].data or self['time_window'].data:
            return False
        return True
    
    def default(self, field, default = ''):
        if not self[field].data:
            return default
        else:
            return str(self[field].data)
        
    def args(self):
        return "source_se=%s&dest_se=%s&state=%s&vo=%s&time_window=%s" %\
            (self.default('source_se'), self.default('dest_se'),
             self.default('state'), self.default('vo'),
             self.default('time_window', 12))
