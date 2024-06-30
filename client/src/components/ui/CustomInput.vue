<template>
  <div class="container">
    <label for="label" class="label">{{ label }}</label>
    <input :id="label" :value="modelValue" class="input" :type="type" @input="onChangeValue"/>
  </div>
</template>
<script lang="ts" setup>

defineProps<{
  modelValue: string
  label: string
  type: string
}>()

const emit = defineEmits<{
  (e: 'update:modelValue', value: string): void
}>()

const onChangeValue = (e: any) => {
  emit('update:modelValue', e.target?.value)
}

</script>
<style lang="scss" scoped>
.container {
  display: flex;
  flex-direction: column;

  .label {
    font-size: 14px;
    margin-bottom: 4px;
  }

  .input {
    border: gray;
    border-radius: 10px;
    outline: none;
    background: white;
    padding: 14px 10px;
  }
}
</style>