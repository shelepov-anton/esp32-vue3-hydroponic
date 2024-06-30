<template>
  <div class="modal">
    <CustomInput v-model="inputValue" label="Кол-во дней" type="number" :style="{ marginBottom: '40px' }" />
    <CustomButton @click="onStartClick" type="primary">
      Старт
    </CustomButton>
  </div>
</template>
<script setup lang="ts">
import CustomButton from '@/components/ui/CustomButton.vue'
import CustomInput from '@/components/ui/CustomInput.vue'
import { useConfigurationStore } from '@/store/configuration'
import { ref } from 'vue'
import { storeToRefs } from 'pinia'

const { daysCount } = storeToRefs(useConfigurationStore())
const inputValue = ref<number>(daysCount.value || '')

const onStartClick = () => {
  daysCount.value = inputValue.value
  localStorage.setItem('daysCount', inputValue.value.toString())
}

</script>
<style lang="scss" scoped>
.modal {
  backdrop-filter: blur(10px);
  padding: 20px;
  width: 90%;
  height: 50%;
  margin: 60% auto;
  border-radius: 10px;
  background: linear-gradient(rgba(0, 92, 151, 0.56), rgba(54, 55, 149, 0.52));
}
</style>